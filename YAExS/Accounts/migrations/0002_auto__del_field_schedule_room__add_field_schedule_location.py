# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):
        # Deleting field 'Schedule.room'
        db.delete_column('Accounts_schedule', 'room')

        # Adding field 'Schedule.location'
        db.add_column('Accounts_schedule', 'location',
                      self.gf('django.db.models.fields.CharField')(default=0, max_length=300),
                      keep_default=False)


    def backwards(self, orm):

        # User chose to not deal with backwards NULL issues for 'Schedule.room'
        raise RuntimeError("Cannot reverse this migration. 'Schedule.room' and its values cannot be restored.")
        # Deleting field 'Schedule.location'
        db.delete_column('Accounts_schedule', 'location')


    models = {
        'Accounts.schedule': {
            'Meta': {'object_name': 'Schedule'},
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'location': ('django.db.models.fields.CharField', [], {'max_length': '300'}),
            'section': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'schedule'", 'to': "orm['courses.Section']"}),
            'timeslot': ('django.db.models.fields.IntegerField', [], {})
        },
        'Accounts.user': {
            'Meta': {'object_name': 'User', '_ormbases': ['auth.User']},
            'user_ptr': ('django.db.models.fields.related.OneToOneField', [], {'to': "orm['auth.User']", 'unique': 'True', 'primary_key': 'True'})
        },
        'auth.group': {
            'Meta': {'object_name': 'Group'},
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'name': ('django.db.models.fields.CharField', [], {'unique': 'True', 'max_length': '80'}),
            'permissions': ('django.db.models.fields.related.ManyToManyField', [], {'to': "orm['auth.Permission']", 'symmetrical': 'False', 'blank': 'True'})
        },
        'auth.permission': {
            'Meta': {'ordering': "('content_type__app_label', 'content_type__model', 'codename')", 'unique_together': "(('content_type', 'codename'),)", 'object_name': 'Permission'},
            'codename': ('django.db.models.fields.CharField', [], {'max_length': '100'}),
            'content_type': ('django.db.models.fields.related.ForeignKey', [], {'to': "orm['contenttypes.ContentType']"}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'name': ('django.db.models.fields.CharField', [], {'max_length': '50'})
        },
        'auth.user': {
            'Meta': {'object_name': 'User'},
            'date_joined': ('django.db.models.fields.DateTimeField', [], {'default': 'datetime.datetime.now'}),
            'email': ('django.db.models.fields.EmailField', [], {'max_length': '75', 'blank': 'True'}),
            'first_name': ('django.db.models.fields.CharField', [], {'max_length': '30', 'blank': 'True'}),
            'groups': ('django.db.models.fields.related.ManyToManyField', [], {'to': "orm['auth.Group']", 'symmetrical': 'False', 'blank': 'True'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'is_active': ('django.db.models.fields.BooleanField', [], {'default': 'True'}),
            'is_staff': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'is_superuser': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'last_login': ('django.db.models.fields.DateTimeField', [], {'default': 'datetime.datetime.now'}),
            'last_name': ('django.db.models.fields.CharField', [], {'max_length': '30', 'blank': 'True'}),
            'password': ('django.db.models.fields.CharField', [], {'max_length': '128'}),
            'user_permissions': ('django.db.models.fields.related.ManyToManyField', [], {'to': "orm['auth.Permission']", 'symmetrical': 'False', 'blank': 'True'}),
            'username': ('django.db.models.fields.CharField', [], {'unique': 'True', 'max_length': '30'})
        },
        'contenttypes.contenttype': {
            'Meta': {'ordering': "('name',)", 'unique_together': "(('app_label', 'model'),)", 'object_name': 'ContentType', 'db_table': "'django_content_type'"},
            'app_label': ('django.db.models.fields.CharField', [], {'max_length': '100'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'model': ('django.db.models.fields.CharField', [], {'max_length': '100'}),
            'name': ('django.db.models.fields.CharField', [], {'max_length': '100'})
        },
        'courses.course': {
            'Meta': {'ordering': "['department__code', 'number']", 'object_name': 'Course'},
            'department': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'courses'", 'to': "orm['courses.Department']"}),
            'description': ('django.db.models.fields.TextField', [], {'default': "''"}),
            'grade_type': ('django.db.models.fields.CharField', [], {'default': "''", 'max_length': '150', 'blank': 'True'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'is_comm_intense': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'max_credits': ('django.db.models.fields.IntegerField', [], {}),
            'min_credits': ('django.db.models.fields.IntegerField', [], {}),
            'name': ('django.db.models.fields.CharField', [], {'max_length': '200'}),
            'number': ('django.db.models.fields.IntegerField', [], {}),
            'prereqs': ('django.db.models.fields.TextField', [], {'default': "''"}),
            'semesters': ('django.db.models.fields.related.ManyToManyField', [], {'related_name': "'courses'", 'symmetrical': 'False', 'through': "orm['courses.OfferedFor']", 'to': "orm['courses.Semester']"}),
            'status': ('django.db.models.fields.CharField', [], {'default': "'unknown'", 'max_length': '100'})
        },
        'courses.department': {
            'Meta': {'ordering': "['code']", 'object_name': 'Department'},
            'code': ('django.db.models.fields.CharField', [], {'unique': 'True', 'max_length': '50'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'name': ('django.db.models.fields.CharField', [], {'default': "''", 'max_length': '200', 'blank': 'True'}),
            'semesters': ('django.db.models.fields.related.ManyToManyField', [], {'related_name': "'departments'", 'symmetrical': 'False', 'through': "orm['courses.SemesterDepartment']", 'to': "orm['courses.Semester']"})
        },
        'courses.offeredfor': {
            'Meta': {'unique_together': "(('course', 'semester'),)", 'object_name': 'OfferedFor'},
            'course': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'offered_for'", 'to': "orm['courses.Course']"}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'ref': ('django.db.models.fields.CharField', [], {'db_index': 'True', 'max_length': '200', 'blank': 'True'}),
            'semester': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'offers'", 'to': "orm['courses.Semester']"})
        },
        'courses.period': {
            'Meta': {'unique_together': "(('start', 'end', 'days_of_week_flag'),)", 'object_name': 'Period'},
            'days_of_week_flag': ('django.db.models.fields.IntegerField', [], {}),
            'end': ('django.db.models.fields.TimeField', [], {'default': 'None', 'null': 'True'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'start': ('django.db.models.fields.TimeField', [], {'default': 'None', 'null': 'True'})
        },
        'courses.section': {
            'Meta': {'ordering': "['number']", 'object_name': 'Section'},
            'course': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'sections'", 'to': "orm['courses.Course']"}),
            'crn': ('django.db.models.fields.IntegerField', [], {'unique': 'True'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'notes': ('django.db.models.fields.TextField', [], {'blank': 'True'}),
            'number': ('django.db.models.fields.CharField', [], {'max_length': '5'}),
            'periods': ('django.db.models.fields.related.ManyToManyField', [], {'related_name': "'sections'", 'symmetrical': 'False', 'through': "orm['courses.SectionPeriod']", 'to': "orm['courses.Period']"}),
            'seats_taken': ('django.db.models.fields.IntegerField', [], {}),
            'seats_total': ('django.db.models.fields.IntegerField', [], {}),
            'semester': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'sections'", 'to': "orm['courses.Semester']"}),
            'visited': ('django.db.models.fields.IntegerField', [], {'default': '0'})
        },
        'courses.sectionperiod': {
            'Meta': {'unique_together': "(('period', 'section', 'semester'),)", 'object_name': 'SectionPeriod'},
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'instructor': ('django.db.models.fields.CharField', [], {'max_length': '150', 'blank': 'True'}),
            'kind': ('django.db.models.fields.CharField', [], {'max_length': '75'}),
            'location': ('django.db.models.fields.CharField', [], {'max_length': '150', 'blank': 'True'}),
            'period': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'section_times'", 'to': "orm['courses.Period']"}),
            'section': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'section_times'", 'to': "orm['courses.Section']"}),
            'semester': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'section_times'", 'to': "orm['courses.Semester']"})
        },
        'courses.semester': {
            'Meta': {'ordering': "['-year', '-month']", 'unique_together': "(('year', 'month'),)", 'object_name': 'Semester'},
            'date_updated': ('django.db.models.fields.DateTimeField', [], {'auto_now': 'True', 'blank': 'True'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'month': ('django.db.models.fields.IntegerField', [], {}),
            'name': ('django.db.models.fields.CharField', [], {'max_length': '100'}),
            'ref': ('django.db.models.fields.CharField', [], {'unique': 'True', 'max_length': '150'}),
            'visible': ('django.db.models.fields.BooleanField', [], {'default': 'True'}),
            'year': ('django.db.models.fields.IntegerField', [], {})
        },
        'courses.semesterdepartment': {
            'Meta': {'unique_together': "(('department', 'semester'),)", 'object_name': 'SemesterDepartment'},
            'department': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'+'", 'to': "orm['courses.Department']"}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'semester': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'+'", 'to': "orm['courses.Semester']"})
        }
    }

    complete_apps = ['Accounts']